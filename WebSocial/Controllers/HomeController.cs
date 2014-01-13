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
    /// <summary>
    /// The Home Controller
    /// </summary>
    public class HomeController : BaseController
    {
        /// <summary>
        /// The database Context
        /// </summary>
        private GameContext db = new GameContext();

        // GET: /
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

        // GET: /About
        public ActionResult About()
        {
            ViewBag.Message = "Your application description page.";

            return View();
        }

        // GET: /Contact
        public ActionResult Contact()
        {
            ViewBag.Message = "Your contact page.";

            return View();
        }
        public ActionResult Hangman()
        {
            return View();

        }

        // GET: /Statistics
        public async Task<ActionResult> Statistics()
        {
            UserDimension users = await Services.GetAllUsers();
            ViewBag.Dimension = users.users.Count;

            UserDimension userStrenght = await Services.GetAllUsersStrenght();
            ViewBag.Strenght = userStrenght.users;

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

                ViewBag.AuthUserTag = GetAuthenticatedUserTagCount(graph.nodes.Count, friendIds);
            }

            return View();
        }

        public async Task<ActionResult> InsertWord(string word)
        {
            ViewBag.Message = "Palavra inserida com sucesso!";

            await Services.AddWordToHangman(word);
            return View("Hangman");
        }

        /// <summary>
        /// The method that will set the culture of the entire website
        /// </summary>
        /// <param name="culture">the string with the culture result</param>
        /// <returns></returns>
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

        /// <summary>
        /// The method to get the overall user tag count
        /// </summary>
        /// <param name="userCount">the number of registered users</param>
        /// <returns>an object that associates the user tag and the count</returns>
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

        /// <summary>
        /// The method to get the overall user tag count for the registered user
        /// </summary>
        /// <param name="userCount">the number of registered friends</param>
        /// <param name="friendIds">the name of the registered friends</param>
        /// <returns>an object that associates the user tag and the count</returns>
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

        /// <summary>
        /// The method to get the overall connection tag count
        /// </summary>
        /// <returns>an object that associates the connection tag and the count</returns>
        private static async Task<List<TagCount>> GetOverallConnectionTagCount()
        {
            TagCountConnection tag = await Services.GetTagCountConnection();

            return ConvertToTagCount(tag);
        }

        /// <summary>
        /// The method to get the overall connection tag count for the registered user
        /// </summary>
        /// <returns>an object that associates the connection tag and the count</returns>
        private static async Task<List<TagCount>> GetAuthenticatedConnectionTagCount(string username)
        {
            TagCountConnection tag = await Services.GetTagCountConnectionByUser(username);

            return ConvertToTagCount(tag);
        }

        /// <summary>
        /// The method that will convert a tag count connection into tag count object used in the html page
        /// </summary>
        /// <param name="tag">the tag count connection object</param>
        /// <returns>the tag count connection object used in html page</returns>
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

        /// <summary>
        /// The method that will calculate the html class for the given tag.
        /// Used by Tag Cloud.
        /// </summary>
        /// <param name="tagCount">the number of the occurences of the given tag</param>
        /// <param name="usersCount">the number of the registered users</param>
        /// <returns>a string with the class of the given tag</returns>
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

        /// <summary>
        /// The method to get a list of the friend id's of the given user graph
        /// </summary>
        /// <param name="graph">the user graph</param>
        /// <returns>a list of the friend id's</returns>
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