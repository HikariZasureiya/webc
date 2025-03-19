#include "headers/url_register.h"

void register_routes(){
    register_route("/" , "templates/index.html"); 
    register_route("/about" , "templates/about.html");
    register_route("/contacts" , "templates/contacts.html");
    //register_route("/register/static" , "templates/contacts.html");
}


