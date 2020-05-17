module Main.Routes exposing (Route(..), fromUrl)

import Browser.Navigation exposing (Key, pushUrl)
import Html exposing (Attribute)
import Html.Attributes
import Url exposing (Url)
import Url.Builder as Builder
import Url.Parser as Parser


type Route
    = Index
    | SeamCarving


parser : Parser.Parser (Route -> a) a
parser =
    Parser.oneOf
        [ Parser.map Index <| Parser.top
        , Parser.map SeamCarving <| Parser.s "sc"
        ]


toString : Route -> String
toString route =
    case route of
        Index ->
            Builder.absolute [] []

        SeamCarving ->
            Builder.absolute [ "sc" ] []


newUrl : Key -> Route -> Cmd msg
newUrl key =
    toString >> pushUrl key


fromUrl : Url -> Maybe Route
fromUrl url =
    Parser.parse parser url


href : Route -> Attribute msg
href route =
    route
        |> toString
        |> Html.Attributes.href
