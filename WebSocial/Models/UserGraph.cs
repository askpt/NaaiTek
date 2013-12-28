using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web;

namespace WebSocial.View
{
    public class Path
    {
        public string user1 { get; set; }
        public string user2 { get; set; }
        public int connection { get; set; }
        public List<string> tags { get; set; }
    }

    public class UserGraph
    {
        public List<string> nodes { get; set; }
        public List<Path> paths { get; set; }
        public string status { get; set; }
    }

    public class UserGraphServices
    {
        private const string _baseUrl = "http://localhost:5000";

        public static async Task<UserGraph> GetUserGraph(string user)
        {
            UserGraph userGraph;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_graph?user={0}", user);

            string responseJson = await client.GetStringAsync(url);

            userGraph = JsonConvert.DeserializeObject<UserGraph>(responseJson);

            return userGraph;
        }
    }
}