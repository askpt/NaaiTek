using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web;

namespace WebSocial.Models
{
    public class UserDim
    {
        public string user { get; set; }
        public int dimension { get; set; }
    }

    public class UserDimension
    {
        public List<UserDim> users { get; set; }
        public string status { get; set; }
    }

    public class UserDimensionServices
    {
        private const string _baseUrl = "http://localhost:5000";

        public static async Task<UserDimension> GetNetworkDimension()
        {
            UserDimension userGraph;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_users_dimension");

            string responseJson = await client.GetStringAsync(url);

            userGraph = JsonConvert.DeserializeObject<UserDimension>(responseJson);

            return userGraph;
        }
    }

}