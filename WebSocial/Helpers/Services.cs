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
    /// <summary>
    /// The class responsible for the services connection (Prolog Webservice connection)
    /// </summary>
    public class Services
    {
        /// <summary>
        /// The base url string of the prolog webservice
        /// </summary>
        private const string _baseUrl = "http://uvm061.dei.isep.ipp.pt:5000";

        /// <summary>
        /// The method to get a list of the dimension network for all users
        /// </summary>
        /// <returns>an ordered list of the dimension of all users</returns>
        public static async Task<UserDimension> GetAllUsers()
        {
            UserDimension userGraph;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_users_dimension");

            string responseJson = await client.GetStringAsync(url);

            userGraph = JsonConvert.DeserializeObject<UserDimension>(responseJson);

            return userGraph;
        }

        /// <summary>
        /// The method to get a list of the strenght network for all users
        /// </summary>
        /// <returns>an ordered list of the strenght of all users </returns>
        public static async Task<UserDimension> GetAllUsersStrenght()
        {
            UserDimension userGraph;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_users_strenght");

            string responseJson = await client.GetStringAsync(url);

            userGraph = JsonConvert.DeserializeObject<UserDimension>(responseJson);

            return userGraph;
        }

        /// <summary>
        /// The method to get a list of the count of the connection tag occurences
        /// </summary>
        /// <returns>a list with a count of all connection tag occurences</returns>
        public static async Task<TagCountConnection> GetTagCountConnection()
        {
            TagCountConnection tagCount;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_tag_count");

            string responseJson = await client.GetStringAsync(url);

            tagCount = JsonConvert.DeserializeObject<TagCountConnection>(responseJson);

            return tagCount;
        }

        /// <summary>
        /// The method to get the user graph friends and 3rd level connections
        /// </summary>
        /// <param name="user">the user username</param>
        /// <returns>a list of the friends and the connections</returns>
        public static async Task<UserGraph> GetUserGraph(string user)
        {
            UserGraph userGraph;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_graph?user={0}", user);

            string responseJson = await client.GetStringAsync(url);

            userGraph = JsonConvert.DeserializeObject<UserGraph>(responseJson);

            return userGraph;
        }

        /// <summary>
        /// The method to get the connection tag count for the given user
        /// </summary>
        /// <param name="username">the user username</param>
        /// <returns>a list of the count of the connection tag</returns>
        public static async Task<TagCountConnection> GetTagCountConnectionByUser(string username)
        {
            TagCountConnection tagCount;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_user_tag_count?user={0}", username);

            string responseJson = await client.GetStringAsync(url);

            tagCount = JsonConvert.DeserializeObject<TagCountConnection>(responseJson);

            return tagCount;
        }
        public static async Task AddWordToHangman(string word)
        {

            HttpClient client = new HttpClient();
            var wordLower = word.ToLower();
            var url = string.Format(_baseUrl + "/add_word_hangman?word={0}", wordLower);

            string responseJson = await client.GetStringAsync(url);
        }

        /// <summary>
        /// The method to get the friend connection to the given user
        /// </summary>
        /// <param name="username">the user username</param>
        /// <returns>a list of the friend connections</returns>
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

        /// <summary>
        /// The method that will edit a friendship by strenght between two users
        /// </summary>
        /// <param name="user1">the first user</param>
        /// <param name="user2">the second user</param>
        /// <param name="strenght">the connection strenght</param>
        /// <returns>true if operation was succeed</returns>
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

        /// <summary>
        /// The method to get all connection tags
        /// </summary>
        /// <returns>a list with all connection tags</returns>
        public static async Task<List<string>> GetAllTags()
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_all_tags");
            string jsonResponse = await client.GetStringAsync(url);

            ConnectionTag tags = JsonConvert.DeserializeObject<ConnectionTag>(jsonResponse);

            return tags.tags;
        }

        /// <summary>
        /// The method that will edit a friendship by adding a tag
        /// </summary>
        /// <param name="user1">the first user</param>
        /// <param name="user2">the second user</param>
        /// <param name="tag">the tag to be added</param>
        /// <returns>true if operation was succeed</returns>
        public static async Task<bool> AddTag(string user1, string user2, string tag)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/add_connection_tag?personA={0}&personB={1}&tag={2}", user1, user2, tag);
            await client.GetAsync(url);

            return true;
        }

        /// <summary>
        /// The method that will edit a friendship by removing a tag
        /// </summary>
        /// <param name="user1">the first user</param>
        /// <param name="user2">the second user</param>
        /// <param name="tag">the tag to be removed</param>
        /// <returns>true if operation was succeed</returns>
        public static async Task<bool> RemoveTag(string user1, string user2, string tag)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/remove_connection_tag?personA={0}&personB={1}&tag={2}", user1, user2, tag);
            await client.GetAsync(url);

            return true;
        }

        /// <summary>
        /// The method that will remove a friendship
        /// </summary>
        /// <param name="user1">the first user</param>
        /// <param name="user2">the second user</param>
        /// <returns>true if operation was succeed</returns>
        public static async Task<bool> RemoveFriend(string user1, string user2)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/remove_connection?personA={0}&personB={1}", user1, user2);
            await client.GetAsync(url);

            return true;
        }

        /// <summary>
        /// The method to get a list of the third level connections of the given username
        /// </summary>
        /// <param name="username">the given username</param>
        /// <returns>a list with the usernames</returns>
        public static async Task<List<string>> GetThirdLevelFriends(string username)
        {
            UserGraph graph = await GetUserGraph(username);

            List<Path> friends = await GetOnlyFriends(username);

            List<string> friendsName = new List<string>();

            friendsName.Add(username);

            foreach (Path item in graph.paths)
            {
                if (item.user1 == username || item.user2 == username)
                {
                    if (item.user1 != username)
                    {
                        friendsName.Add(item.user1);
                    }
                    else
                    {
                        friendsName.Add(item.user2);
                    }
                }
            }

            List<string> thirdFriends = graph.nodes;

            foreach (string item in friendsName)
            {
                thirdFriends.Remove(item);
            }

            return thirdFriends;
        }

        /// <summary>
        /// The method to get all the pending requests of the given user
        /// </summary>
        /// <param name="username">the user username</param>
        /// <returns>a list of the users that make the request</returns>
        public static async Task<List<string>> GetAllPendingRequests(string username)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_all_pending_requests?user={0}", username);
            string jsonResponse = await client.GetStringAsync(url);
            UserRequests user = JsonConvert.DeserializeObject<UserRequests>(jsonResponse);

            return user.users;
        }

        /// <summary>
        /// The method to send a user friend request
        /// </summary>
        /// <param name="user">the user that makes the request</param>
        /// <param name="friend">the user that will receive the request</param>
        /// <returns></returns>
        public static async Task SendUserRequest(string user, string friend)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/request_friend?personA={0}&personB={1}", user, friend);
            await client.GetAsync(url);
        }

        /// <summary>
        /// The method to get all the pending requests of the given user
        /// </summary>
        /// <param name="username">the user username</param>
        /// <returns>a list of the users that make the request</returns>
        public static async Task<List<string>> CheckFriendRequests(string user)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/check_requests?user={0}", user);
            string jsonResponse = await client.GetStringAsync(url);
            UserRequests users = JsonConvert.DeserializeObject<UserRequests>(jsonResponse);

            return users.users;
        }

        /// <summary>
        /// The method to get all the pending game requests of the given user
        /// </summary>
        /// <param name="username">the user username</param>
        /// <returns>a list of the users that respond the request</returns>
        public static async Task<List<string>> CheckGameFriendRequests(string user)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/check_game_requests?user={0}", user);
            string jsonResponse = await client.GetStringAsync(url);
            UserRequests users = JsonConvert.DeserializeObject<UserRequests>(jsonResponse);

            return users.users;
        }

        /// <summary>
        /// The method to get all the accepted requests of the given user
        /// </summary>
        /// <param name="username">the user username</param>
        /// <returns>a list of the users that accepted the request</returns>
        public static async Task<List<string>> CheckAcceptedFriendRequests(string user)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/check_friend_notifications?user={0}", user);
            string jsonResponse = await client.GetStringAsync(url);
            UserRequests users = JsonConvert.DeserializeObject<UserRequests>(jsonResponse);

            return users.users;
        }

        /// <summary>
        /// The method that will delete the friend requests
        /// </summary>
        /// <param name="user">the user username</param>
        /// <param name="friend">the requested friend username</param>
        /// <returns></returns>
        public static async Task DeleteFriendRequest(string user, string friend)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/remove_request?personA={0}&personB={1}", user, friend);
            await client.GetAsync(url);
        }

        /// <summary>
        /// The method that will respond the friend request with the game status
        /// </summary>
        /// <param name="user">the user username</param>
        /// <param name="friend">the requested friend username</param>
        /// <returns></returns>
        public static async Task GameResponseFriendRequest(string user, string friend)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/game_response?personA={0}&personB={1}", user, friend);
            await client.GetAsync(url);
        }

        /// <summary>
        /// The method that will accept an user friend request
        /// </summary>
        /// <param name="user">the user username</param>
        /// <param name="friend">the requested friend username</param>
        /// <returns></returns>
        public static async Task AcceptResponseFriendRequest(string user, string friend)
        {
            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/accept_response?personA={0}&personB={1}", user, friend);
            await client.GetAsync(url);
        }
    }


}