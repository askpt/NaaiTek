using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web;
using WebSocial.Helpers.Models;
using WebSocial.Models;

namespace WebSocial.Helpers
{
    public class Services
    {
        private const string _baseUrl = "http://localhost:5000";

        public static async Task<UserDimension> GetAllUsers()
        {
            UserDimension userGraph;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_users_dimension");

            string responseJson = await client.GetStringAsync(url);

            userGraph = JsonConvert.DeserializeObject<UserDimension>(responseJson);

            return userGraph;
        }

        public static async Task<TagCountConnection> GetTagCountConnection()
        {
            TagCountConnection tagCount;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_tag_count");

            string responseJson = await client.GetStringAsync(url);

            tagCount = JsonConvert.DeserializeObject<TagCountConnection>(responseJson);

            return tagCount;
        }

        public static async Task<UserGraph> GetUserGraph(string user)
        {
            UserGraph userGraph;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_graph?user={0}", user);

            string responseJson = await client.GetStringAsync(url);

            userGraph = JsonConvert.DeserializeObject<UserGraph>(responseJson);

            return userGraph;
        }

        public static async Task<TagCountConnection> GetTagCountConnectionByUser(string username)
        {
            TagCountConnection tagCount;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_user_tag_count?user={0}", username);

            string responseJson = await client.GetStringAsync(url);

            tagCount = JsonConvert.DeserializeObject<TagCountConnection>(responseJson);

            return tagCount;
        }

        public static async Task<List<Path>> GetOnlyFriends(string username)
        {
            UserGraph graph = await GetUserGraph(username);

            List<Path> friends = new List<Path>();

            foreach (Path item in graph.paths)
            {
                if (item.user1 == username || item.user2 == username)
                {
                    friends.Add(item);
                }
            }

            return friends;
        }

        public static async Task<bool> EditConnection(string user1, string user2, int strenght)
        {
            HttpClient client = new HttpClient();
            if (strenght > -1)
            {
                var url = string.Format(_baseUrl + "/update_connection_strenght?personA={0}&personB={1}&strenght={2}", user1, user2, strenght);
                await client.GetAsync(url);
            }

            return true;
        }

        public static async Task<List<string>> GetAllTags()
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_all_tags");
            string jsonResponse = await client.GetStringAsync(url);

            ConnectionTag tags = JsonConvert.DeserializeObject<ConnectionTag>(jsonResponse);

            return tags.tags;
        }

        public static async Task<bool> AddTag(string user1, string user2, string tag)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/add_connection_tag?personA={0}&personB={1}&tag={2}", user1, user2, tag);
            await client.GetAsync(url);

            return true;
        }

        public static async Task<bool> RemoveTag(string user1, string user2, string tag)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/remove_connection_tag?personA={0}&personB={1}&tag={2}", user1, user2, tag);
            await client.GetAsync(url);

            return true;
        }

        public static async Task<bool> RemoveFriend(string user1, string user2)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/remove_connection?personA={0}&personB={1}", user1, user2);
            await client.GetAsync(url);

            return true;
        }
    }


}