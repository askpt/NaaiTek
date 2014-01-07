using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;

namespace DatabaseWs
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IDatabase" in both code and config file together.
    [ServiceContract]
    public interface IDatabase
    {
        [OperationContract]
        [WebInvoke(Method = "GET",
                    ResponseFormat = WebMessageFormat.Json,
                    RequestFormat = WebMessageFormat.Json,
                    UriTemplate = "data")]

        string DoWork();

        [OperationContract]
        [WebInvoke(Method = "GET",
                    ResponseFormat = WebMessageFormat.Json,
                    RequestFormat = WebMessageFormat.Json,
                    UriTemplate = "user/{user}")]
        User GetUser(string user);

        [OperationContract]
        [WebInvoke(Method = "GET",
                    ResponseFormat = WebMessageFormat.Json,
                    RequestFormat = WebMessageFormat.Json,
                    UriTemplate = "auth?user={user}&pass={pass}")]
        Message Authenticate(string user, string pass);
    }
}
