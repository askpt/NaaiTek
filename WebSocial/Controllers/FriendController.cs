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
            }

            return View();
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
        public ActionResult Edit(int id)
        {
            return View();
        }

        //
        // POST: /Friend/Edit/5
        [HttpPost]
        public ActionResult Edit(int id, FormCollection collection)
        {
            try
            {
                // TODO: Add update logic here

                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }

        //
        // GET: /Friend/Delete/5
        public ActionResult Delete(int id)
        {
            return View();
        }

        //
        // POST: /Friend/Delete/5
        [HttpPost]
        public ActionResult Delete(int id, FormCollection collection)
        {
            try
            {
                // TODO: Add delete logic here

                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }
    }
}
