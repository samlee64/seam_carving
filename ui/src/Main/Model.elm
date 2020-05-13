module Main.Model exposing (Model, Page(..), init, initPage, updatePage)

import Browser.Navigation exposing (Key)
import Flags exposing (Flags)
import Main.Msg exposing (Msg(..))
import Main.Routes as Routes
import Page.Index as Index
import Url exposing (Url)


type Page
    = IndexPage Index.Model
    | NotFound


type alias Model =
    { page : Page, flags : Flags }


init : Flags -> Url -> Key -> ( Model, Cmd Msg )
init flags url key =
    let
        ( model, cmd ) =
            initPage url { page = NotFound, flags = flags }
    in
    ( model, cmd )


initPage : Url -> Model -> ( Model, Cmd Msg )
initPage url model =
    case Routes.fromUrl url of
        Just Routes.Index ->
            Index.init |> updatePage IndexPage IndexMsg model

        Nothing ->
            ( { model | page = NotFound }, Cmd.none )


updatePage : (pageModel -> Page) -> (pageMsg -> Msg) -> Model -> ( pageModel, Cmd pageMsg ) -> ( Model, Cmd Msg )
updatePage toPage toMsg model ( pageModel, pageCmd ) =
    ( { model | page = toPage pageModel }
    , Cmd.map toMsg pageCmd
    )
