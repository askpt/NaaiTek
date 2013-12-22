using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace WebSocial.Models
{
    public class ExternalLoginConfirmationViewModel
    {
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "UserNameRequired")]
        [Display(Name = "UserName", ResourceType = typeof(Resources.Resources))]
        public string UserName { get; set; }
    }

    public class ManageUserViewModel
    {
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "CurrentPwRequired")]
        [DataType(DataType.Password)]
        [Display(Name = "CurrentPw", ResourceType = typeof(Resources.Resources))]
        public string OldPassword { get; set; }

        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "NewPwRequired")]
        [StringLength(100, MinimumLength = 6, ErrorMessageResourceType = typeof(Resources.Resources),
                          ErrorMessageResourceName = "PwLong")]
        [DataType(DataType.Password)]
        [Display(Name = "NewPw", ResourceType = typeof(Resources.Resources))]
        public string NewPassword { get; set; }

        [DataType(DataType.Password)]
        [Display(Name = "ConfirmNewPw", ResourceType = typeof(Resources.Resources))]
        [Compare("NewPassword", ErrorMessageResourceType = typeof(Resources.Resources),
                          ErrorMessageResourceName = "MatchPw")]
        public string ConfirmPassword { get; set; }
    }

    public class LoginViewModel
    {
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "UserNameRequired")]
        [Display(Name = "UserName", ResourceType = typeof(Resources.Resources))]
        public string UserName { get; set; }

        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "PwRequired")]
        [DataType(DataType.Password)]
        [Display(Name = "Password", ResourceType = typeof(Resources.Resources))]
        public string Password { get; set; }

        [Display(Name = "RememberMe", ResourceType = typeof(Resources.Resources))]
        public bool RememberMe { get; set; }
    }

    public class RegisterViewModel
    {
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "UserNameRequired")]
        [Display(Name = "UserName", ResourceType = typeof(Resources.Resources))]
        public string UserName { get; set; }


        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "NewPwRequired")]
        [StringLength(100, MinimumLength = 6, ErrorMessageResourceType = typeof(Resources.Resources),
                          ErrorMessageResourceName = "PwLong")]
        [DataType(DataType.Password)]
        [Display(Name = "Pw", ResourceType = typeof(Resources.Resources))]
        public string Password { get; set; }


        [DataType(DataType.Password)]
        [Display(Name = "ConfirmPw", ResourceType = typeof(Resources.Resources))]
        [Compare("Password", ErrorMessageResourceType = typeof(Resources.Resources),
                          ErrorMessageResourceName = "MatchPw")]
        public string ConfirmPassword { get; set; }


        //birth date
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "BDRequired")]
        [Display(Name = "BD", ResourceType = typeof(Resources.Resources))]
        public DateTime BirthDate { get; set; }

        //Email
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "EmailRequired")]
        [RegularExpression(@"[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}",
            ErrorMessageResourceType = typeof(Resources.Resources),
                          ErrorMessageResourceName = "RegExEmail")]
        [Display(Name = "E-mail")]
        public String Email { get; set; }

        //Number
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "ContNumberRequired")]
        [Display(Name = "ContactNumber", ResourceType = typeof(Resources.Resources))]
        public int Number { get; set; }

        //Country
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "CountryRequired")]
        [Display(Name = "Country", ResourceType = typeof(Resources.Resources))]
        public String Country { get; set; }

        //City  
        [Required(ErrorMessageResourceType = typeof(Resources.Resources),
                  ErrorMessageResourceName = "CityRequired")]
        [Display(Name = "City", ResourceType = typeof(Resources.Resources))]
        public String City { get; set; }
    }
}
