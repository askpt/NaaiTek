namespace WebSocial.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class removedTagInheritance : DbMigration
    {
        public override void Up()
        {
            DropColumn("dbo.Tags", "TotalUsers");
            DropColumn("dbo.Tags", "Discriminator");
        }
        
        public override void Down()
        {
            AddColumn("dbo.Tags", "Discriminator", c => c.String(nullable: false, maxLength: 128));
            AddColumn("dbo.Tags", "TotalUsers", c => c.Int());
        }
    }
}
