module Data.Markings exposing (..)

import Json.Decode exposing (..)
import Json.Encode as E


type alias Markings =
    { destroy : String, protect : String }



--{ destroy : List Int, protect : List Int }


markingsDecoder : Decoder Markings
markingsDecoder =
    map2 Markings
        (at [ "detail", "destroy" ] string)
        (at [ "detail", "protect" ] string)



--        (at [ "detail", "destroy" ] (list int))
--        (at [ "detail", "protect" ] (list int))


encode : Markings -> E.Value
encode markings =
    E.object
        [ ( "destroy", E.string markings.destroy )
        , ( "protect", E.string markings.protect )
        ]



--        [ ( "destroy", E.list E.int markings.destroy )
--        , ( "protect", E.list E.int markings.protect )
--        ]
