using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Web;

namespace WebSocial.Models
{
    /// <summary>
    /// The class that will create a relationship between user and tag
    /// </summary>
    public class UserTag
    {
        /// <summary>
        /// The relationship ID
        /// </summary>
        [Key]
        public int ID { get; set; }

        /// <summary>
        /// The user ID
        /// </summary>
        public string UserID { get; set; }

        /// <summary>
        /// The tag ID
        /// </summary>
        public int TagID { get; set; }
    }
}