module View.WebData exposing (..)

import Bootstrap.Alert as Alert
import Bootstrap.Button as Button
import Html exposing (..)
import Http exposing (Error(..))
import RemoteData as RD exposing (RemoteData(..), WebData)


viewWebData : (a -> Html msg) -> WebData a -> Html msg
viewWebData view webdata =
    case webdata of
        Success a ->
            view a

        Loading ->
            Alert.simpleWarning [] [ text "Loading" ]

        NotAsked ->
            Alert.simpleSecondary [] [ text "Not Asked" ]

        Failure e ->
            Alert.simpleDanger [] [ viewHttpError e ]


viewHttpError : Error -> Html msg
viewHttpError e =
    case e of
        BadUrl s ->
            text s

        Timeout ->
            text "Timeout"

        NetworkError ->
            text "Network Error"

        BadStatus int ->
            text <| "Bad Status: " ++ String.fromInt int

        BadBody s ->
            text s


viewWebDataButton : List (Button.Option msg) -> List (Html msg) -> WebData a -> Html msg
viewWebDataButton options view data =
    if RD.isLoading data then
        Button.button (Button.disabled True :: options) [ text "Loading" ]

    else
        Button.button options view
