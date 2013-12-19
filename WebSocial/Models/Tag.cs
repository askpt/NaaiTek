using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.ComponentModel.DataAnnotations;

namespace WebSocial.Models
{
    public class Tag
    {
        [Key]
        public int ID { get; set; }

        public String Name { get; set; }
    }
}