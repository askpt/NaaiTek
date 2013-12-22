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

        [Display(Name = "Name", ResourceType = typeof(Resources.Resources))]
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "NameRequired")]
        [StringLength(50, ErrorMessageResourceType = typeof(Resources.Resources),
                          ErrorMessageResourceName = "NameLong")]
        public String Name { get; set; }
    }
}