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
        [Authorize(Roles = "User")]
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
            List<string> pendingRequests = await Services.GetAllPendingRequests(username);

            suggestedFriends = await GetFriendsSuggestedThirdLevel(username, userTagIDs, pendingRequests);

            if (suggestedFriends.Count == 0)
            {
                suggestedFriends = await GetAllFriendsSuggested(username, userTagIDs, pendingRequests);
            }

            return suggestedFriends;
        }

        private async Task<List<string>> GetAllFriendsSuggested(string username, IList<int> userTagIDs, List<string> pendingRequests)
        {
            List<string> suggestedFriends = new List<string>();
            List<string> allUsers = await GetAllUsers(username);

            foreach (string item in pendingRequests)
            {
                if (allUsers.Contains(item))
                {
                    allUsers.Remove(item);
                }
            }

            List<string> allUsersId = FindFriendIds(allUsers);

            foreach (string friendId in allUsersId)
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
                suggestedFriends = allUsers;
            }

            return suggestedFriends;
        }

        private async Task<List<string>> GetAllUsers(string username)
        {
            UserDimension usersDim = await Services.GetAllUsers();
            List<UserDim> allUsers = usersDim.users;

            List<string> usernames = new List<string>();
            foreach (UserDim item in allUsers)
            {
                if (item.user != username)
                {
                    usernames.Add(item.user);
                }
            }

            return usernames;
        }

        private async Task<List<string>> GetFriendsSuggestedThirdLevel(string username, IList<int> userTagIDs, List<string> pendingRequests)
        {
            List<string> suggestedFriends = new List<string>();

            List<string> thirdLevelFriends = await Services.GetThirdLevelFriends(username);

            foreach (string item in pendingRequests)
            {
                if (thirdLevelFriends.Contains(item))
                {
                    thirdLevelFriends.Remove(item);
                }
            }

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
        [Authorize(Roles = "User")]
        public ActionResult Details(int id)
        {
            return View();
        }

        //
        // GET: /Friend/Create
        [Authorize(Roles = "User")]
        public ActionResult Create()
        {
            return View();
        }

        //
        // POST: /Friend/Create
        [HttpPost]
        [Authorize(Roles = "User")]
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
        [Authorize(Roles = "User")]
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
        [Authorize(Roles = "User")]
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
        [Authorize(Roles = "User")]
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
        [Authorize(Roles = "User")]
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

        public async Task<ActionResult> RequestFriend(string friend)
        {
            string userID = User.Identity.GetUserId();

            if (userID != null)
            {
                ApplicationUser user = db.Users.Find(userID);

                await Services.SendUserRequest(user.UserName, friend);
            }

            return RedirectToAction("Index");
        }

        [Authorize(Roles = "User")]
        public async Task<ActionResult> Notification()
        {
            string userID = User.Identity.GetUserId();

            if (userID != null)
            {
                ApplicationUser user = db.Users.Find(userID);

                // received friend requests
                ViewBag.pendingRequests = await Services.CheckFriendRequests(user.UserName);

                //received game responses
                ViewBag.gameRequests = await Services.CheckGameFriendRequests(user.UserName);

                //check accepted requests notifications
                ViewBag.requestNotification = await Services.CheckAcceptedFriendRequests(user.UserName);
            }

            return View();
        }

    }
}
