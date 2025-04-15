struct Coord {
    int x ;
    int y ;
} ;

struct set_value_peticion {
    int key;
    string value1<256>;
    int N_value2;
    double V_value2[32];
    struct Coord value3;
} ;


struct get_value_respuesta {
    int status;
    string value1<256>;
    int N_value2;
    double V_value2[32];
    struct Coord value3;
} ;

program CLAVES {
    version CLAVESVER {

        int destroy                          ( void ) = 1;

        int set_value                        ( set_value_peticion p ) = 2;

        struct get_value_respuesta get_value ( int key ) = 3;

        int modify_value                     ( set_value_peticion p ) = 4;

        int delete_key                       ( int key ) = 5;

        int exist                            ( int key ) = 6;

    } = 1;
} = 100454255;
