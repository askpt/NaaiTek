using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web;

namespace WebSocial.Helpers.Models
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
}