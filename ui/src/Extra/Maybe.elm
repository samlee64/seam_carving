module Extra.Maybe exposing (..)


isJust : Maybe a -> Bool
isJust val =
    case val of
        Just _ ->
            True

        Nothing ->
            False


isNothing : Maybe a -> Bool
isNothing val =
    case val of
        Just _ ->
            False

        Nothing ->
            True
