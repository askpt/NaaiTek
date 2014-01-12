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
    /// <summary>
    /// The class that will implements the webservice interface
    /// </summary>
    public class Database : IDatabase
    {

        /// <summary>
        /// Method to test if the webservice works
        /// </summary>
        /// <returns>a "Test" string</returns>
        public string DoWork()
        {
            return "Test";
        }

        /// <summary>
        /// Method to get the user information
        /// </summary>
        /// <param name="user">the user username</param>
        /// <returns>the data of the user</returns>
        public User GetUser(string user)
        {
            User ret = null;

            using (var db = new DatabaseAccess())
            {
                ret = db.GetUser(user);
            }


            return ret;
        }

        /// <summary>
        /// Method that will authenticate the user in the system
        /// </summary>
        /// <param name="username">the username of the user</param>
        /// <param name="pass">the password of the user</param>
        /// <returns>an object with the result of the authentication</returns>
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

        /// <summary>
        /// Method to verify if the password is the same of the hashed password
        /// </summary>
        /// <param name="password">the password</param>
        /// <param name="hashedPassword">the hashed password</param>
        /// <returns>true if the hashed password is the same of the hashed password generated with the password</returns>
        private bool ValidateCredentials(string password, string hashedPassword)
        {
            var doesPasswordMatch = Crypto.VerifyHashedPassword(hashedPassword, password);
            return doesPasswordMatch;
        }
    }

    /// <summary>
    /// The return user class
    /// </summary>
    public class User
    {
        /// <summary>
        /// Birth day
        /// </summary>
        public int Day { get; set; }
        /// <summary>
        /// Birth month
        /// </summary>
        public int Month { get; set; }
        /// <summary>
        /// Birth year
        /// </summary>
        public int Year { get; set; }
        /// <summary>
        /// User username
        /// </summary>
        public string Username { get; set; }
        /// <summary>
        /// Phone number
        /// </summary>
        public int? Number { get; set; }
        /// <summary>
        /// User email
        /// </summary>
        public string Email { get; set; }
        /// <summary>
        /// User country
        /// </summary>
        public string Country { get; set; }
        /// <summary>
        /// User City
        /// </summary>
        public string City { get; set; }
        /// <summary>
        /// The number of tags of the user
        /// </summary>
        public int numTags { get; set; }
    }

    /// <summary>
    /// The message return object
    /// </summary>
    public class Message
    {
        /// <summary>
        /// a string with status message
        /// </summary>
        public string Status { get; set; }
        /// <summary>
        /// a string with the message of the message object
        /// </summary>
        public string Msg { get; set; }
    }

}
