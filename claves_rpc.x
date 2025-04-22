struct Coord_rpc {
    int x ;
    int y ;
} ;

struct set_value_peticion {
    int key;
    string value1<256>;
    int N_value2;
    double V_value2[32];
    struct Coord_rpc value3;
} ;


struct get_value_respuesta {
    int status;
    string value1<256>;
    int N_value2;
    double V_value2[32];
    struct Coord_rpc value3;
} ;

program CLAVES {
    version CLAVESVER {

        int destroy_server                          ( void ) = 1;

        int set_value_server                        ( set_value_peticion p ) = 2;

        struct get_value_respuesta get_value_server ( int key ) = 3;

        int modify_value_server                     ( set_value_peticion p ) = 4;

        int delete_key_server                       ( int key ) = 5;

        int exist_server                            ( int key ) = 6;

    } = 1;
} = 100454255;
