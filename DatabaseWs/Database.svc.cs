using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;

namespace DatabaseWs
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Database" in code, svc and config file together.
    // NOTE: In order to launch WCF Test Client for testing this service, please select Database.svc or Database.svc.cs at the Solution Explorer and start debugging.
    public class Database : IDatabase
    {
        public string DoWork()
        {
            return "Test";
        }

        public User GetUser(string user)
        {
            User ret = null;

            using (var db = new UserContext())
            {
                foreach (AspNetUsers item in db.AspNetUsers)
                {
                    if (item.UserName == user)
                    {
                        ret = new User() 
                        {
                            Day = item.BirthDate.Value.Day, 
                            Month = item.BirthDate.Value.Month, 
                            Year = item.BirthDate.Value.Year,
                            Username = item.UserName,
                            City = item.City,
                            Country = item.Country,
                            Email = item.Email,
                            Number = item.Number                        
                        };
                    }
                }
            }


            return ret;
        }
    }

    public class User
    {
        public int Day { get; set; }
        public int Month { get; set; }
        public int Year { get; set; }
        public string Username { get; set; }
        public int? Number { get; set; }
        public string Email { get; set; }
        public string Country { get; set; }
        public string City { get; set; }
    }

}
