namespace WebSocial.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class birthDate2 : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.AspNetUsers", "BirthDate", c => c.DateTime());
        }
        
        public override void Down()
        {
            DropColumn("dbo.AspNetUsers", "BirthDate");
        }
    }
}
