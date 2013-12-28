using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebSocial.Models
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
}