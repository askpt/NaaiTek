using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Security.Cryptography;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Web.Helpers;

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

            using (var db = new DatabaseAccess())
            {
                ret = db.GetUser(user);
            }


            return ret;
        }

        public Message Authenticate(string username, string pass)
        {
            Message ret = null;
            bool isAuthenticated = false;

            using (var db = new DatabaseAccess())
            {
                string hashPwd = db.GetPasswordHash(username);

                isAuthenticated = ValidateCredentials(pass, hashPwd);
            }

            if (isAuthenticated)
            {
                ret = new Message() { Status = "ok", Msg = "User Authenticated" };
            }
            else
            {
                ret = new Message() { Status = "fail", Msg = "Password Fail" };
            }

            return ret;
        }


        public bool ValidateCredentials(string password, string hashedPassword)
        {
            var doesPasswordMatch = Crypto.VerifyHashedPassword(hashedPassword, password);
            return doesPasswordMatch;
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

    public class Message
    {
        public string Status { get; set; }
        public string Msg { get; set; }
    }

}
