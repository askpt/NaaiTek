using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using WebSocial.Models;

namespace WebSocial.Helpers.Models
{
    /// <summary>
    /// The class that will relates a certain user Tag and the number of users that the class uses
    /// </summary>
    public class TagCount
    {
        /// <summary>
        /// A tag object
        /// </summary>
        public Tag Tag { get; set; }
        /// <summary>
        /// The number of the users of the given tag
        /// </summary>
        public int TotalUsers { get; set; }
        /// <summary>
        /// The string responsible for the HTML class of the given tag
        /// </summary>
        public string TagClass { get; set; }
    }

    /// <summary>
    /// The object that will relates the connection tag and the number of the connections that uses the connection tag
    /// </summary>
    public class TagConnection
    {
        /// <summary>
        /// The connection tag
        /// </summary>
        public string tag { get; set; }
        /// <summary>
        /// The number of the occurences
        /// </summary>
        public int count { get; set; }
    }

    /// <summary>
    /// The root object of the count of the connection tags
    /// </summary>
    public class TagCountConnection
    {
        /// <summary>
        /// A list of connection tag count
        /// </summary>
        public List<TagConnection> tags { get; set; }
        /// <summary>
        /// The number of all connections
        /// </summary>
        public int nr_connections { get; set; }
        /// <summary>
        /// The Json status message
        /// </summary>
        public string status { get; set; }
    }
}
