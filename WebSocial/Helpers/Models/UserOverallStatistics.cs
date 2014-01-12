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
    /// The object that relates the user and the network dimension
    /// </summary>
    public class UserDim
    {
        /// <summary>
        /// The user name
        /// </summary>
        public string user { get; set; }
        /// <summary>
        /// The user network dimension
        /// </summary>
        public int dimension { get; set; }
    }

    /// <summary>
    /// The root object that contains a list of the objects that relates the user and the network dimension
    /// </summary>
    public class UserDimension
    {
        /// <summary>
        /// The list of object
        /// </summary>
        public List<UserDim> users { get; set; }
        /// <summary>
        /// The Json status message
        /// </summary>
        public string status { get; set; }
    }
}