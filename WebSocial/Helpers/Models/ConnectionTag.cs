using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebSocial.Helpers.Models
{
    /// <summary>
    /// A helper class that will have the list of all tags
    /// </summary>
    public class ConnectionTag
    {
        /// <summary>
        /// The list of all tags
        /// </summary>
        public List<string> tags { get; set; }
        /// <summary>
        /// The json status message
        /// </summary>
        public string status { get; set; }
    }
}