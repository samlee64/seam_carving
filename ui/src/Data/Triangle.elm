module Data.Triangle exposing (..)

import Json.Encode as E


type alias Triangle =
    { one : List Int
    , two : List Int
    , three : List Int
    }


shiftRight : Triangle -> List Int -> Triangle
shiftRight tri coords =
    { one = tri.two
    , two = tri.three
    , three = coords
    }


addCoord : Triangle -> List Int -> Result String Triangle
addCoord tri coords =
    if List.isEmpty tri.one then
        Ok { tri | one = coords }

    else if List.isEmpty tri.two then
        Ok { tri | two = coords }

    else if List.isEmpty tri.three then
        Ok { tri | three = coords }

    else
        Err "Triangle is complete"


isEmpty : Triangle -> Bool
isEmpty tri =
    List.isEmpty tri.one && List.isEmpty tri.two && List.isEmpty tri.three


isComplete : Triangle -> Bool
isComplete tri =
    let
        fn =
            List.isEmpty >> not
    in
    fn tri.one && fn tri.two && fn tri.three


empty : Triangle
empty =
    { one = [], two = [], three = [] }



--formTriangle : IncompleteTriangle -> Maybe Triangle
--completeTriangle : IncompleteTriangle -> Maybe Triangle
--completeTriangle tri =
--    Maybe.map3 (\one two three -> { one = one, two = two, three = three }) tri.one tri.two tri.three


encode : Triangle -> E.Value
encode tri =
    E.list (E.list E.int) [ tri.one, tri.two, tri.three ]
