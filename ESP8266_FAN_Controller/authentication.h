//authentication ONLY FOR SYSTEM CONFIG
bool authSystemPages(AsyncWebServerRequest * request)
{
  // if(!request->authenticate(userName,userPass))
  //      return;
  if (securityLevel > 0)
    if (!request->authenticate(userName, userPass))
    {
      request->requestAuthentication(AUTH_REQ, true);
      //   request->send(401, "text/html", "<html><body><h1> Unauthorized</h1></body><html>");
      return false;
    }
  return true;
}

//authentication ALL PAGES
bool authUserPages(AsyncWebServerRequest * request)
{
  // if(!request->authenticate(userName,userPass))
  //      return;
  if (securityLevel > 1)
    if (!request->authenticate(userName, userPass))
    {
      request->requestAuthentication(AUTH_REQ, true);
      // request->send(401, "text/html", "<html><body><h1> Unauthorized</h1></body><html>");
      return false;
    }
  return true;
}
