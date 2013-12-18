using WebSocial.Models;
using Microsoft.AspNet.Identity.EntityFramework;
using System.Data.Entity;
using WebSocial.Models;

namespace WebSocial.DAL
{
    public class GameContext : IdentityDbContext<ApplicationUser>
    {
        public GameContext()
            : base("GANDALF061")
        {

        }

    }
}