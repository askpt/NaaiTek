using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace WebSocial.Models
{
    public class TagCount
    {
        public Tag Tag { get; set; }
        public int TotalUsers { get; set; }
        public string TagClass { get; set; }
    }

    public class TagConnection
    {
        public string tag { get; set; }
        public int count { get; set; }
    }

    public class TagCountConnection
    {
        public List<TagConnection> tags { get; set; }
        public int nr_connections { get; set; }
        public string status { get; set; }
    }

    public class TagCountServices
    {
        private const string _baseUrl = "http://localhost:5000";

        public static async Task<TagCountConnection> GetTagCountConnection()
        {
            TagCountConnection tagCount;

            HttpClient client = new HttpClient();

            var url = string.Format(_baseUrl + "/get_tag_count");

            string responseJson = await client.GetStringAsync(url);

            tagCount = JsonConvert.DeserializeObject<TagCountConnection>(responseJson);

            return tagCount;
        }
    }
}
