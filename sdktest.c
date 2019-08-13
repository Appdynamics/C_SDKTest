// ************************************************************************************************************
// ******                                                                                                ******
// ****** PROGRAM: sdktest.cpp                                                                           ******
// ****** AUTHOR:  Robb Kane                                                                             ******
// ****** DATE:    August, 11, 2019                                                                      ******
// ******                                                                                                ******
// ****** PROPERTY OF APPDYNAMICS, INC.  ALL RIGHTS RESERVED.                                            ******
// ******                                                                                                ******
// ****** This program is example code only.  There is no warranties implied or otherwise regarding its  ******
// ****** performance.  AppDynamics is not responsible for its use, nor any consequence of its use.      ******
// ******                                                                                                ******
// ************************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "appdynamics.h"
#include "appdynamics_advanced.h"

#define FALSE (1==0)

int main(int argc, char** argv)
{
    int            init_timeout = -1;
    int            pause = 1;
    int            rc = 0;

    appd_bt_handle        bt_handle = NULL;
    struct appd_config*   cfg = NULL;

    unsigned long  bt_begin_calls_made = 0;
    unsigned long  bt_end_calls_made = 0;
    unsigned long  limit = 10000;
    unsigned long  loop = 0;
    unsigned long  controller_port = 0;
    unsigned long  proxy_port = 0;

    unsigned       use_SSL = FALSE;
    unsigned       use_cert_file = FALSE;
    unsigned       use_proxy = FALSE;

    char*          cert_file = NULL;
    char*          cert_dir = NULL;

    char*          controller_host = NULL;
    char*          acct_name = "customer1";

    char*          app_name = "SDKTestApp1";
    char*          tier_name = "SDKTestTier1";
    char*          node_name = "SDKTestNode1";

    char*          bt_name = "SDKTestBT1";

    char*          proxy_host = NULL;
    char*          proxy_user = NULL;
    char*          proxy_pswd = NULL;
    char*          proxy_pswd_file = NULL;

    char*          access_key = "Access Key Not Set";

    if (argc > 1)
    {
		controller_host = argv[1];
    }

    if (argc > 2)
    {
		unsigned int temp = atoi(argv[2]);
		controller_port = temp & 0xFFFF;
    }

    if (argc > 3)
    {
		access_key = argv[3];
    }

    if (argc > 4)
    {
		acct_name = argv[4];
    }

    printf("Controller Wait Timeout is %d seconds\n", (init_timeout/1000)); 
    printf("Controller Host is \"%s\"\n",             controller_host);
    printf("Controller Port is %lu\n",                controller_port);
    printf("Controller useSSL is %s\n",               use_SSL ? "true" : "false");
    printf("Controller Application is \"%s\"\n",      app_name);
    printf("Controller Tier is \"%s\"\n",             tier_name);
    printf("Controller Node is \"%s\"\n",             node_name);
    printf("Controller Account Name is \"%s\"\n",     acct_name);
    printf("Controller License is \"%s\"\n",          access_key);

   cfg = appd_config_init();

   if (cfg == NULL)
   {
       printf("appd_config_init() failed!\n");
       exit(1);
   }

   appd_config_set_controller_host(cfg, controller_host);
   appd_config_set_controller_port(cfg, controller_port);
   appd_config_set_controller_account(cfg, acct_name);
   appd_config_set_controller_access_key(cfg, access_key);
   appd_config_set_controller_use_ssl(cfg, use_SSL);
   appd_config_set_init_timeout_ms(cfg, 10000);
   appd_config_set_app_name(cfg, app_name);
   appd_config_set_tier_name(cfg, tier_name);
   appd_config_set_node_name(cfg, node_name);

   if (use_cert_file)
   {
       appd_config_set_controller_certificate_file(cfg, cert_file);
       appd_config_set_controller_certificate_dir( cfg, cert_dir);
   }

   if (use_proxy)
   {
       appd_config_set_controller_http_proxy_host(         cfg, proxy_host);
       appd_config_set_controller_http_proxy_port(         cfg, proxy_port & 0xFFFF);
       appd_config_set_controller_http_proxy_username(     cfg, proxy_user);
       appd_config_set_controller_http_proxy_password(     cfg, proxy_pswd);
       appd_config_set_controller_http_proxy_password_file(cfg, proxy_pswd_file);
   }

   printf("About to call appd_sdk_init(), waiting for Controller response...\n");

   rc = appd_sdk_init(cfg);

   if (rc != 0)
   {
       printf("appd_sdk_init() failed!\n");
       exit(1);
   }

   for (loop = 0; loop < limit; loop++)
   {
        sleep(pause);
		printf("Calling appd_bt_begin()...");
        bt_handle = appd_bt_begin(bt_name, NULL);
        bt_begin_calls_made++;
		printf("handle = %p\n", bt_handle);
        appd_bt_end(bt_handle);
        bt_end_calls_made++;
   }

    appd_sdk_term();

    printf("All SDK API calls have completed.\n\n");
    printf("BT Begin Calls Made:         %lu\n", bt_begin_calls_made);
    printf("BT End   Calls Made:         %lu\n", bt_end_calls_made);

}

// end of sdktest..cpp file.
