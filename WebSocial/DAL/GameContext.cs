using WebSocial.Models;
using Microsoft.AspNet.Identity.EntityFramework;
using System.Data.Entity;

namespace WebSocial.DAL
{
    public class GameContext : IdentityDbContext<ApplicationUser>
    {
        public GameContext()
            : base("GANDALF061")
        {

        }

        public System.Data.Entity.DbSet<Tag> Tags { get; set; }
        public System.Data.Entity.DbSet<UserTag> UsersTags { get; set; }

    }
}