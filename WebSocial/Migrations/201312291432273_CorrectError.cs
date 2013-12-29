namespace WebSocial.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class CorrectError : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.Tags", "TotalUsers", c => c.Int());
            AddColumn("dbo.Tags", "Discriminator", c => c.String(nullable: false, maxLength: 128));
        }
        
        public override void Down()
        {
            DropColumn("dbo.Tags", "Discriminator");
            DropColumn("dbo.Tags", "TotalUsers");
        }
    }
}
