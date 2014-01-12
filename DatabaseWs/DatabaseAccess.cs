using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Web;

namespace DatabaseWs
{
    /// <summary>
    /// Class used to access the database
    /// </summary>
    public class DatabaseAccess : IDisposable
    {
        /// <summary>
        /// The database url string
        /// </summary>
        private const string _dbUrl = @"data source=gandalf.dei.isep.ipp.pt\sqlexpress;initial catalog=ARQSI061;user id=ARQSI061;password=ARQSI061";

        /// <summary>
        /// The database connection object
        /// </summary>
        private SqlConnection con;

        /// <summary>
        /// The main constructor of the database access class
        /// </summary>
        public DatabaseAccess()
        {
            con = new SqlConnection(_dbUrl);
            con.Open();
        }

        /// <summary>
        /// Method to get the hashed password of the given user
        /// </summary>
        /// <param name="username">the username of the user</param>
        /// <returns>the hashed password of the given user</returns>
        public string GetPasswordHash(string username)
        {
            string hashPwd = "";

            string queryString = "select PasswordHash from dbo.AspNetUsers where UserName='" + username + "'";

            SqlCommand cmd = new SqlCommand(queryString, con);
            SqlDataReader reader = cmd.ExecuteReader();
            while (reader.Read())
            {
                hashPwd = (string)reader[0];
            }

            return hashPwd;
        }

        /// <summary>
        /// Method to get the data of the given user
        /// </summary>
        /// <param name="username">the username of the user</param>
        /// <returns>an object with the data of the given user</returns>
        public User GetUser(string username)
        {
            User user = new User();

            string queryString = "SELECT u.UserName, u.Email, u.Number, u.Country, u.City, u.BirthDate, "+
                                 "(SELECT Count(ut.TagId) " +
                                 "FROM dbo.AspNetUsers u, dbo.UserTags ut " +
                                 "WHERE u.UserName='" + username + "' AND u.Id = ut.UserID) AS countUserTags " +
                                 "FROM dbo.AspNetUsers u " +
                                 "WHERE u.UserName='" + username + "'";

            SqlCommand cmd = new SqlCommand(queryString, con);

            SqlDataReader reader = cmd.ExecuteReader();
            while (reader.Read())
            {
                user.Username = (string)reader[0];
                user.Email = (string)reader[1];
                user.Number = (int)reader[2];
                user.Country = (string)reader[3];
                user.City = (string)reader[4];

                DateTime date = (DateTime)reader[5];
                user.Day = date.Day;
                user.Month = date.Month;
                user.Year = date.Year;

                user.numTags = (int)reader[6];
            }

            return user;
        }

        /// <summary>
        /// Method to close the connection to the database
        /// </summary>
        public void Dispose()
        {
            con.Close();
        }
    }
}