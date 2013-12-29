﻿using System;
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

namespace WebSocial.Controllers
{
    public class HomeController : BaseController
    {

        private GameContext db = new GameContext();

        public async Task<ActionResult> Index()
        {
            string userID = User.Identity.GetUserId();
            ApplicationUser user = db.Users.Find(userID);

            IList<int> tagIDs = (from userToTag in db.UsersTags where (userToTag.UserID == userID) select userToTag.TagID).ToList();
            IQueryable<Tag> tags = (from tag in db.Tags where tagIDs.Contains(tag.ID) select tag);
            ViewBag.UserTags = tags;

            string username = user.UserName;
            UserGraph graph = await UserGraphServices.GetUserGraph(username);

            // need to remove one because this dimension contains self user
            int dimension = (graph.nodes.Count - 1);

            ViewBag.Dimension = dimension;

            return View(user);
        }

        public ActionResult About()
        {
            ViewBag.Message = "Your application description page.";

            return View();
        }

        public ActionResult Contact()
        {
            ViewBag.Message = "Your contact page.";

            return View();
        }

        public async Task<ActionResult> Statistics()
        {
            UserDimension users = await UserDimensionServices.GetNetworkDimension();
            ViewBag.Dimension = users.users.Count;

            ViewBag.Users = users.users;

            ViewBag.AnomUserTag = GetOverallUserTagCount(users.users.Count);

            return View();
        }

        public ActionResult SetCulture(string culture)
        {
            // Validate input
            culture = CultureHelper.GetImplementedCulture(culture);
            // Save culture in a cookie
            HttpCookie cookie = Request.Cookies["_culture"];
            if (cookie != null)
                cookie.Value = culture;   // update cookie value
            else
            {
                cookie = new HttpCookie("_culture");
                cookie.Value = culture;
                cookie.Expires = DateTime.Now.AddYears(1);
            }
            Response.Cookies.Add(cookie);
            return RedirectToAction("Index");
        }

        private List<TagCount> GetOverallUserTagCount(int userCount)
        {
            List<Tag> allTags = db.Tags.ToList();
            List<TagCount> tagsStatistics = new List<TagCount>();
            List<UserTag> allUsers = db.UsersTags.ToList();

            foreach (Tag tag in allTags)
            {
                TagCount tagTemp = new TagCount { Tag = tag, TotalUsers = 0 };
                foreach (UserTag user in allUsers)
                {
                    if (tag.ID == user.TagID)
                    {
                        tagTemp.TotalUsers++;
                    }
                }
                tagTemp.TagClass = GetTagClass(tagTemp.TotalUsers, userCount);
                tagsStatistics.Add(tagTemp);
            }

            return tagsStatistics;
        }

        public static string GetTagClass(int tagCount, int usersCount)
        {
            var result = (tagCount * 100) / usersCount;
            if (result <= 1)
                return "tag1";
            if (result <= 4)
                return "tag2";
            if (result <= 8)
                return "tag3";
            if (result <= 12)
                return "tag4";
            if (result <= 18)
                return "tag5";
            if (result <= 30)
                return "tag6";
            return result <= 50 ? "tag7" : "";
        }

    }
}