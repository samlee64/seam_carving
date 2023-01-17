module Main.Routes exposing (Route(..), fromUrl, toString)

import Browser.Navigation exposing (Key, pushUrl)
import Html exposing (Attribute)
import Html.Attributes
import Url exposing (Url)
import Url.Builder as Builder
import Url.Parser as Parser exposing (..)
import Url.Parser.Query as Query


type Route
    = Index
    | SeamCarving
    | SeamCarvingPicture (Maybe String)
    | Technologies


parser : Parser.Parser (Route -> a) a
parser =
    Parser.oneOf
        [ Parser.map Index <| Parser.top
        , Parser.map SeamCarvingPicture <| Parser.s "seam-carving" <?> Query.string "w"
        , Parser.map SeamCarving <| Parser.s "seam-carving"
        , Parser.map Technologies <| Parser.s "technologies"
        ]


toString : Route -> String
toString route =
    case route of
        Index ->
            Builder.absolute [] []

        SeamCarvingPicture pictureUrl ->
            Builder.absolute [ "seam-carving", "picture" ] []

        SeamCarving ->
            Builder.absolute [ "seam-carving" ] []

        Technologies ->
            Builder.absolute [ "technologies" ] []


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
