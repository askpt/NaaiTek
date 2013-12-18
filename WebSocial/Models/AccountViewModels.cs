using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace WebSocial.Models
{
    public class ExternalLoginConfirmationViewModel
    {
        [Required]
        [Display(Name = "User name")]
        public string UserName { get; set; }
    }

    public class ManageUserViewModel
    {
        [Required]
        [DataType(DataType.Password)]
        [Display(Name = "Current password")]
        public string OldPassword { get; set; }

        [Required]
        [StringLength(100, ErrorMessage = "The {0} must be at least {2} characters long.", MinimumLength = 6)]
        [DataType(DataType.Password)]
        [Display(Name = "New password")]
        public string NewPassword { get; set; }

        [DataType(DataType.Password)]
        [Display(Name = "Confirm new password")]
        [Compare("NewPassword", ErrorMessage = "The new password and confirmation password do not match.")]
        public string ConfirmPassword { get; set; }
    }

    public class LoginViewModel
    {
        [Required]
        [Display(Name = "User name")]
        public string UserName { get; set; }

        [Required]
        [DataType(DataType.Password)]
        [Display(Name = "Password")]
        public string Password { get; set; }

        [Display(Name = "Remember me?")]
        public bool RememberMe { get; set; }
    }

    public class RegisterViewModel
    {
        [Required]
        [Display(Name = "User name")]
        public string UserName { get; set; }

        [Required]
        [StringLength(100, ErrorMessage = "The {0} must be at least {2} characters long.", MinimumLength = 6)]
        [DataType(DataType.Password)]
        [Display(Name = "Password")]
        public string Password { get; set; }

        [DataType(DataType.Password)]
        [Display(Name = "Confirm password")]
        [Compare("Password", ErrorMessage = "The password and confirmation password do not match.")]
        public string ConfirmPassword { get; set; }


        //birth date
        [Required(ErrorMessage = "Birth Date is required")]
        [Display(Name = "Birth Date in this form \"YYYY-MM-DD\"")]
        public DateTime BirthDate { get; set; }

        //Email
        [Required(ErrorMessage = "Email is required")]
        [RegularExpression(@"[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}",
            ErrorMessage = "Invalid e-mail inserted.")]
        [Display(Name = "E-mail")]
        public String Email { get; set; }

        //Tags
        [Display(Name = "Tags")]
        public List<String> Tags { get; set; }

        //Number
        [Display(Name = "Contact Number")]
        public int Number { get; set; }

        //Country
        [Required(ErrorMessage = "Country is required")]
        [Display(Name = "Country")]
        public String Country { get; set; }

        //City  
        [Required(ErrorMessage = "City is required")]
        [Display(Name = "City")]
        public String City { get; set; }
    }
}
