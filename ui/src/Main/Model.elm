module Main.Model exposing (Model, Page(..), init, initPage, updatePage)

import Bootstrap.Navbar as Navbar
import Browser.Navigation as Navigation exposing (Key)
import Flags exposing (Flags)
import Main.Msg exposing (Msg(..))
import Main.Routes as Routes
import Page.Index as Index
import Page.SeamCarving.Model as SeamCarving
import Url exposing (Url)


type Page
    = IndexPage Index.Model
    | SeamCarvingPage SeamCarving.Model
    | NotFound


type alias Model =
    { page : Page
    , flags : Flags
    , key : Key
    , navbarState : Navbar.State
    }


init : Flags -> Url -> Key -> ( Model, Cmd Msg )
init flags url key =
    let
        ( navbarState, nCmd ) =
            Navbar.initialState NavbarMsg

        ( model, cmd ) =
            initPage url { page = NotFound, flags = flags, key = key, navbarState = navbarState }
    in
    ( model, Cmd.batch [ cmd, nCmd ] )


initPage : Url -> Model -> ( Model, Cmd Msg )
initPage url model =
    case Routes.fromUrl url of
        Just Routes.Index ->
            Index.init model.flags |> updatePage IndexPage IndexMsg model

        Just Routes.SeamCarving ->
            SeamCarving.init model.flags |> updatePage SeamCarvingPage SeamCarvingMsg model

        Nothing ->
            ( { model | page = NotFound }, Cmd.none )


updatePage : (pageModel -> Page) -> (pageMsg -> Msg) -> Model -> ( pageModel, Cmd pageMsg ) -> ( Model, Cmd Msg )
updatePage toPage toMsg model ( pageModel, pageCmd ) =
    ( { model | page = toPage pageModel }
    , Cmd.map toMsg pageCmd
    )
