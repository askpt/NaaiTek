namespace WebSocial.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class otherUserAttributes : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.AspNetUsers", "Email", c => c.String());
            AddColumn("dbo.AspNetUsers", "Number", c => c.Int());
            AddColumn("dbo.AspNetUsers", "Country", c => c.String());
            AddColumn("dbo.AspNetUsers", "City", c => c.String());
        }
        
        public override void Down()
        {
            DropColumn("dbo.AspNetUsers", "City");
            DropColumn("dbo.AspNetUsers", "Country");
            DropColumn("dbo.AspNetUsers", "Number");
            DropColumn("dbo.AspNetUsers", "Email");
        }
    }
}
