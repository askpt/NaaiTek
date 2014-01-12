using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebSocial.Helpers.Models
{
    /// <summary>
    /// The helper class model that will be responsible for the user friend requests
    /// </summary>
    public class UserRequests
    {
        /// <summary>
        /// The list of usernames of usernames that sent a request to the registered user
        /// </summary>
        public List<string> users { get; set; }

        /// <summary>
        /// The status code of Json Object
        /// </summary>
        public string status { get; set; }
    }
}