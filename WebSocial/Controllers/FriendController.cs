using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using WebSocial.Helpers;
using Microsoft.AspNet.Identity;
using Microsoft.AspNet.Identity.EntityFramework;
using WebSocial.DAL;
using WebSocial.Models;
using System.Threading.Tasks;
using WebSocial.Helpers.Models;
using System.Net;

namespace WebSocial.Controllers
{
    public class FriendController : BaseController
    {
        private GameContext db = new GameContext();

        //
        // GET: /Friend/
        public async Task<ActionResult> Index()
        {
            string userID = User.Identity.GetUserId();

            if (userID != null)
            {
                ApplicationUser user = db.Users.Find(userID);

                List<Path> friends = await Services.GetOnlyFriends(user.UserName);

                ViewBag.friends = friends;
                ViewBag.username = user.UserName;

                ViewBag.suggestedFriends = await GetSuggestedFriends(user.UserName, userID);

            }

            return View();
        }

        private async Task<List<string>> GetSuggestedFriends(string username, string userId)
        {
            List<string> suggestedFriends = new List<string>();
            IList<int> userTagIDs = (from userToTag in db.UsersTags where (userToTag.UserID == userId) select userToTag.TagID).ToList();

            suggestedFriends = await GetFriendsSuggestedThirdLevel(username, userTagIDs);

            return suggestedFriends;
        }

        private async Task<List<string>> GetFriendsSuggestedThirdLevel(string username, IList<int> userTagIDs)
        {
            List<string> suggestedFriends = new List<string>();

            List<string> thirdLevelFriends = await Services.GetThirdLevelFriends(username);

            if (thirdLevelFriends.Count > 0)
            {
                List<string> thirdLevelFriendsId = FindFriendIds(thirdLevelFriends);

                foreach (string friendId in thirdLevelFriendsId)
                {
                    IList<int> friendTagIds = (from userToTag in db.UsersTags where (userToTag.UserID == friendId) select userToTag.TagID).ToList();

                    if (userTagIDs.Intersect(friendTagIds).ToList().Count > 0)
                    {
                        ApplicationUser friend = db.Users.Find(friendId);

                        suggestedFriends.Add(friend.UserName);
                    }
                }

                if (suggestedFriends.Count == 0)
                {
                    suggestedFriends = thirdLevelFriends;
                }
            }

            return suggestedFriends;
        }

        private List<string> FindFriendIds(List<string> usernames)
        {
            List<string> userIds = new List<string>();

            foreach (ApplicationUser item in db.Users)
            {
                if (usernames.Contains(item.UserName))
                {
                    userIds.Add(item.Id);
                }
            }

            return userIds;
        }

        //
        // GET: /Friend/Details/5
        public ActionResult Details(int id)
        {
            return View();
        }

        //
        // GET: /Friend/Create
        public ActionResult Create()
        {
            return View();
        }

        //
        // POST: /Friend/Create
        [HttpPost]
        public ActionResult Create(FormCollection collection)
        {
            try
            {
                // TODO: Add insert logic here

                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }

        //
        // GET: /Friend/Edit/5
        public async Task<ActionResult> Edit(int id)
        {
            string userID = User.Identity.GetUserId();

            if (userID != null)
            {
                ApplicationUser user = db.Users.Find(userID);

                List<Path> friends = await Services.GetOnlyFriends(user.UserName);

                ViewBag.username = user.UserName;
                ViewBag.allTags = await Services.GetAllTags();
                ViewBag.last = friends.Count - 1;

                return View(friends[id]);
            }

            return View();
        }

        //
        // POST: /Friend/Edit/5
        [HttpPost]
        public async Task<ActionResult> Edit(int id, FormCollection collection)
        {
            try
            {
                string userID = User.Identity.GetUserId();

                if (userID != null)
                {
                    int strenght = int.Parse(collection["strenghtList"]);

                    string user1 = collection["user1"];

                    string user2 = collection["user2"];

                    await Services.EditConnection(user1, user2, strenght);
                }

                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }

        //
        // GET: /Friend/Delete/5
        public async Task<ActionResult> Delete(int id)
        {
            string userID = User.Identity.GetUserId();

            if (userID != null)
            {
                ApplicationUser user = db.Users.Find(userID);

                List<Path> friends = await Services.GetOnlyFriends(user.UserName);
                ViewBag.username = user.UserName;

                return View(friends[id]);
            }

            return View();
        }

        //
        // POST: /Friend/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<ActionResult> DeleteConfirmed(int id, FormCollection collection)
        {
            try
            {
                string user1 = collection["user1"];

                string user2 = collection["user2"];

                await Services.RemoveFriend(user1, user2);

                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }

        public bool ConnectionHasTag(string allTag, List<string> tags)
        {
            return (tags.Contains(allTag));
        }

        public async Task<ActionResult> AddTag(string tag, string user1, string user2, int last)
        {
            await Services.AddTag(user1, user2, tag);
            return RedirectToAction("Edit/" + last);
        }

        public async Task<ActionResult> RemoveTag(string tag, string user1, string user2, int last)
        {
            await Services.RemoveTag(user1, user2, tag);
            return RedirectToAction("Edit/" + last);
        }
    }
}
