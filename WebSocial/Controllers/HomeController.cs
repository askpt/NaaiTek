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

            if (userID != null)
            {
                string username = user.UserName;
                UserGraph graph = await Services.GetUserGraph(username);

                // need to remove one because this dimension contains self user
                int dimension = (graph.nodes.Count - 1);

                ViewBag.Dimension = dimension;
            }

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
            UserDimension users = await Services.GetAllUsers();
            ViewBag.Dimension = users.users.Count;

            ViewBag.Users = users.users;

            ViewBag.AnomUserTag = GetOverallUserTagCount(users.users.Count);

            ViewBag.AnomConnTag = await GetOverallConnectionTagCount();

            // authenticated users
            string userID = User.Identity.GetUserId();
            if (userID != null && User.IsInRole("User"))
            {
                ApplicationUser user = db.Users.Find(userID);

                ViewBag.AuthConnTag = await GetAuthenticatedConnectionTagCount(user.UserName);

                UserGraph graph = await Services.GetUserGraph(user.UserName);
                List<string> friendIds = FindFriendIds(graph);

                ViewBag.AnomUserTag = GetAuthenticatedUserTagCount(graph.nodes.Count, friendIds);
            }

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
                foreach (UserTag userTag in allUsers)
                {
                    if (tag.ID == userTag.TagID)
                    {
                        tagTemp.TotalUsers++;
                    }
                }
                tagTemp.TagClass = GetTagClass(tagTemp.TotalUsers, userCount);
                tagsStatistics.Add(tagTemp);
            }

            return tagsStatistics;
        }

        private List<TagCount> GetAuthenticatedUserTagCount(int userCount, List<string> friendIds)
        {
            List<Tag> allTags = db.Tags.ToList();
            List<TagCount> tagsStatistics = new List<TagCount>();
            List<UserTag> allUsers = db.UsersTags.ToList();

            foreach (Tag tag in allTags)
            {
                TagCount tagTemp = new TagCount { Tag = tag, TotalUsers = 0 };
                foreach (UserTag userTag in allUsers)
                {
                    if (tag.ID == userTag.TagID && friendIds.Contains(userTag.UserID))
                    {
                        tagTemp.TotalUsers++;
                    }
                }
                tagTemp.TagClass = GetTagClass(tagTemp.TotalUsers, userCount);
                tagsStatistics.Add(tagTemp);
            }

            return tagsStatistics;
        }

        private static async Task<List<TagCount>> GetOverallConnectionTagCount()
        {
            TagCountConnection tag = await Services.GetTagCountConnection();

            return ConvertToTagCount(tag);
        }

        private static async Task<List<TagCount>> GetAuthenticatedConnectionTagCount(string username)
        {
            TagCountConnection tag = await Services.GetTagCountConnectionByUser(username);

            return ConvertToTagCount(tag);
        }
        private static List<TagCount> ConvertToTagCount(TagCountConnection tag)
        {
            List<TagCount> listTags = new List<TagCount>();

            // Convert the Tag Count Connection to the Tag Count object
            foreach (TagConnection item in tag.tags)
            {
                string classTag = GetTagClass(item.count, tag.nr_connections);
                Tag tagTmp = new Tag() { Name = item.tag };
                TagCount tagCountTmp = new TagCount() { Tag = tagTmp, TotalUsers = item.count, TagClass = classTag };

                listTags.Add(tagCountTmp);
            }

            return listTags;
        }

        private static string GetTagClass(int tagCount, int usersCount)
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

        private List<string> FindFriendIds(UserGraph graph)
        {
            List<string> usernames = graph.nodes;
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

    }
}