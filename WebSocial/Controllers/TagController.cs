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

namespace WebSocial.Controllers
{
    public class TagController : Controller
    {
        private GameContext db = new GameContext();

        // GET: /Tag/
        public async Task<ActionResult> Index()
        {
            return View(await db.Tags.ToListAsync());
        }

        // GET: /Tag/Create
        public ActionResult Create()
        {
            return View();
        }

        // POST: /Tag/Create
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
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
        public async Task<ActionResult> DeleteConfirmed(int id)
        {
            Tag tag = await db.Tags.FindAsync(id);
            db.Tags.Remove(tag);
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

        // GET: /Tag/AddOrRemoveTag/5
        public async Task<ActionResult> AddOrRemoveTag(int id)
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
                bool userHasThisTag;
                userHasThisTag = (userTags.Contains(userID)) ? true : false;

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
            }
            // only option is to add it
            else
            {
                db.UsersTags.Add(ut);
            }

            await db.SaveChangesAsync();
            return RedirectToAction("Index");
        }
    }
}
