using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(WebSocial.Startup))]
namespace WebSocial
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
        }
    }
}
