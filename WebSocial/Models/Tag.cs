using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.ComponentModel.DataAnnotations;

namespace WebSocial.Models
{
    /// <summary>
    /// The class that will be responsible to the tag properties
    /// </summary>
    public class Tag
    {
        /// <summary>
        /// The tag ID
        /// </summary>
        [Key]
        public int ID { get; set; }

        /// <summary>
        /// The tag names
        /// </summary>
        [Display(Name = "Name", ResourceType = typeof(Resources.Resources))]
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "NameRequired")]
        [StringLength(50, ErrorMessageResourceType = typeof(Resources.Resources),
                          ErrorMessageResourceName = "NameLong")]
        public String Name { get; set; }
    }
}