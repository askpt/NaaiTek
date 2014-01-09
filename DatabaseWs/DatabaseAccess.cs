using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Web;

namespace DatabaseWs
{
    public class DatabaseAccess : IDisposable
    {
        private const string _dbUrl = @"data source=gandalf.dei.isep.ipp.pt\sqlexpress;initial catalog=ARQSI061;user id=ARQSI061;password=ARQSI061";

        private SqlConnection con;

        public DatabaseAccess()
        {
            con = new SqlConnection(_dbUrl);
            con.Open();
        }

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


        public void Dispose()
        {
            con.Close();
        }
    }
}