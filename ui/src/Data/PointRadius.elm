module Data.PointRadius exposing (..)

import Json.Encode as E


type alias PointRadius =
    { point : List Int
    , radius : Int
    }


encode : PointRadius -> E.Value
encode pr =
    E.object
        [ ( "point", E.list E.int pr.point )
        , ( "radius", E.int pr.radius )
        ]
