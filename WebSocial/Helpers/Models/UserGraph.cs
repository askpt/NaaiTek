using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web;

namespace WebSocial.Helpers.Models
{
    /// <summary>
    /// The class that contains a friend relationship information
    /// </summary>
    public class Path
    {
        /// <summary>
        /// The first user of the relationship
        /// </summary>
        public string user1 { get; set; }
        /// <summary>
        /// The second user of the relationship
        /// </summary>
        public string user2 { get; set; }
        /// <summary>
        /// The connection strenght
        /// </summary>
        public int connection { get; set; }
        /// <summary>
        /// A list with connection tags
        /// </summary>
        public List<string> tags { get; set; }
    }

    /// <summary>
    /// The root object that contains a list of friends and the list of relationship information
    /// </summary>
    public class UserGraph
    {
        /// <summary>
        /// The list of the friends
        /// </summary>
        public List<string> nodes { get; set; }
        /// <summary>
        /// The list of the connection information
        /// </summary>
        public List<Path> paths { get; set; }
        /// <summary>
        /// The json status message
        /// </summary>
        public string status { get; set; }
    }
}