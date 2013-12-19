namespace WebSocial.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class noListInUser : DbMigration
    {
        public override void Up()
        {
            DropForeignKey("dbo.Tags", "ApplicationUser_Id", "dbo.AspNetUsers");
            DropIndex("dbo.Tags", new[] { "ApplicationUser_Id" });
            DropColumn("dbo.Tags", "ApplicationUser_Id");
        }
        
        public override void Down()
        {
            AddColumn("dbo.Tags", "ApplicationUser_Id", c => c.String(maxLength: 128));
            CreateIndex("dbo.Tags", "ApplicationUser_Id");
            AddForeignKey("dbo.Tags", "ApplicationUser_Id", "dbo.AspNetUsers", "Id");
        }
    }
}
