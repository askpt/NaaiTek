namespace WebSocial.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class userIdString : DbMigration
    {
        public override void Up()
        {
            AlterColumn("dbo.UserTags", "UserID", c => c.String());
        }
        
        public override void Down()
        {
            AlterColumn("dbo.UserTags", "UserID", c => c.Int(nullable: false));
        }
    }
}
