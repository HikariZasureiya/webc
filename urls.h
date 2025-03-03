#include "helper/url_dist.h"


void register_routes(){
    register_route("/" , "templates/index.html"); 
    register_route("/about" , "templates/about.html");
    register_route("/contacts" , "templates/contacts.html");
    
}

