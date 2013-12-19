using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Web;

namespace WebSocial.Models
{
    public class UserTag
    {
        [Key]
        public int ID { get; set; }

        public string UserID { get; set; }

        public int TagID { get; set; }
    }
}