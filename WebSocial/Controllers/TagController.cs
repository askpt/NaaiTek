using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Threading.Tasks;
using System.Net;
using System.Web;
using System.Web.Mvc;
using WebSocial.Models;
using WebSocial.DAL;
using Microsoft.AspNet.Identity;
using Microsoft.AspNet.Identity.EntityFramework;
using WebSocial.Helpers;

namespace WebSocial.Controllers
{
    /// <summary>
    /// The controller class for the tag
    /// </summary>
    public class TagController : BaseController
    {
        /// <summary>
        /// The database context
        /// </summary>
        private GameContext db = new GameContext();

        // GET: /Tag/
        public async Task<ActionResult> Index()
        {
            // gets current user id
            string userID = User.Identity.GetUserId();

            IList<int> tagIDs = (from userToTag in db.UsersTags where (userToTag.UserID == userID) select userToTag.TagID).ToList();
            IQueryable<Tag> tags = (from tag in db.Tags where tagIDs.Contains(tag.ID) select tag);
            ViewBag.UserTags = tags;
            return View(await db.Tags.ToListAsync());
        }

        // GET: /Tag/Create
        [Authorize (Roles="Admin, User")]
        public ActionResult Create()
        {
            return View();
        }

        // POST: /Tag/Create
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Admin, User")]
        public async Task<ActionResult> Create([Bind(Include = "ID,Name")] Tag tag)
        {
            if (ModelState.IsValid)
            {
                db.Tags.Add(tag);
                await db.SaveChangesAsync();
                return RedirectToAction("Index");
            }

            return View(tag);
        }

        // GET: /Tag/Delete/5
        [Authorize(Roles = "Admin")]
        public async Task<ActionResult> Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            Tag tag = await db.Tags.FindAsync(id);
            if (tag == null)
            {
                return HttpNotFound();
            }
            return View(tag);
        }

        // POST: /Tag/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Admin")]
        public async Task<ActionResult> DeleteConfirmed(int id)
        {
            Tag tag = await db.Tags.FindAsync(id);
            db.Tags.Remove(tag);
            // delete rows from UsersTags table
            IEnumerable<UserTag> usersTagsToDelete = (from userTagToDelete in db.UsersTags where userTagToDelete.TagID == id select userTagToDelete);
            db.UsersTags.RemoveRange(usersTagsToDelete);

            await db.SaveChangesAsync();
            return RedirectToAction("Index");
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                db.Dispose();
            }
            base.Dispose(disposing);
        }

        /// <summary>
        /// the method that will check if the user has the selected tag
        /// </summary>
        /// <param name="id">the tag id</param>
        /// <returns>returns true if the user has the selected tag</returns>
        public bool UserHasTag(int id)
        {
            // gets current user id
            string userID = User.Identity.GetUserId();

            //creates the tag to be added/removed
            UserTag ut = new UserTag();
            ut.TagID = id;
            ut.UserID = userID;

            // gets all the users with the current tag
            IList<string> userTags = (from tag in db.UsersTags where (tag.TagID == id) select tag.UserID).ToList();

            // if it doesnt exit any user with that tag
            if (!userTags.Count.Equals(0))
            {
                // checks if the current user is one of those who has the tag
                if (userTags.Contains(userID))
                {
                    return true;
                }
            }
            // only option is to add it
            return false;
        }

        // GET: /Tag/AddOrRemoveTag/5
        [Authorize(Roles="User")]
        public async Task<ActionResult> AddOrRemoveTag(int id)
        {
            // gets current user id
            string userID = User.Identity.GetUserId();

            //creates the tag to be added/removed
            UserTag ut = new UserTag();
            ut.TagID = id;
            ut.UserID = userID;

            // checks if the current user is one of those who has the tag
            bool userHasThisTag = UserHasTag(id);

            // if he has the tag, removes it
            if (userHasThisTag)
            {
                IQueryable<UserTag> remTagQuery = (from remTag in db.UsersTags where remTag.TagID == id && remTag.UserID == userID select remTag);
                db.UsersTags.Remove(remTagQuery.First());
            }
            // if he doesnt, adds it
            else
            {
                db.UsersTags.Add(ut);
            }

            await db.SaveChangesAsync();
            return RedirectToAction("Index");
        }

    }
}
