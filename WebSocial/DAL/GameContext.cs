using WebSocial.Models;
using Microsoft.AspNet.Identity.EntityFramework;
using System.Data.Entity;

namespace WebSocial.DAL
{
    /// <summary>
    /// The class that wil be responsible for the database connection
    /// </summary>
    public class GameContext : IdentityDbContext<ApplicationUser>
    {
        /// <summary>
        /// The main constructor of the database connection class
        /// </summary>
        public GameContext()
            : base("GANDALF061")
        {

        }

        /// <summary>
        /// A list of user tags
        /// </summary>
        public System.Data.Entity.DbSet<Tag> Tags { get; set; }
        /// <summary>
        /// A list of the relationship between user and tags
        /// </summary>
        public System.Data.Entity.DbSet<UserTag> UsersTags { get; set; }

    }
}