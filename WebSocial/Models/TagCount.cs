using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WebSocial.Models
{
    public class TagCount
    {
        public Tag Tag { get; set; }
        public int TotalUsers { get; set; }
        public string TagClass { get; set; }
    }
}
