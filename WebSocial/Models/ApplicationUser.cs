using Microsoft.AspNet.Identity.EntityFramework;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Web;

namespace WebSocial.Models
{
    // You can add profile data for the user by adding more properties to your ApplicationUser class, please visit http://go.microsoft.com/fwlink/?LinkID=317594 to learn more.

    /// <summary>
    /// The class that will be responsible to the User information
    /// </summary>
    public class ApplicationUser : IdentityUser
    {
        /// <summary>
        /// User birth date
        /// </summary>
        public DateTime BirthDate { get; set; }

        /// <summary>
        /// User email
        /// </summary>
        public String Email { get; set; }

        /// <summary>
        /// The user number
        /// </summary>
        public int Number { get; set; }

        /// <summary>
        /// The user country
        /// </summary>
        public String Country { get; set; }

        /// <summary>
        /// The user city
        /// </summary>
        public String City { get; set; }
    }
}