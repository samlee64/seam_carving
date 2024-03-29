module Main.View exposing (..)

import Bootstrap.Button as Button
import Bootstrap.Form.Input as Input
import Bootstrap.Grid as Grid
import Bootstrap.Navbar as Navbar
import Bootstrap.Utilities.Spacing as Spacing
import Browser exposing (Document)
import Html exposing (..)
import Html.Attributes exposing (class, href, placeholder, src, style)
import Html.Events exposing (onClick)
import Main.Model exposing (Model, Page(..))
import Main.Msg exposing (Msg(..))
import Main.Routes as Routes
import Page.Index as Index
import Page.SeamCarving.View as SeamCarving
import Page.Technologies as Technologies


view : Model -> Document Msg
view model =
    { title = "Sam Lee"
    , body = [ viewNavbar model, view_ model ]
    }


view_ : Model -> Html Msg
view_ model =
    div
        [ style "position" "aboslute"
        , style "height" "100vh"
        , style "width" "100vw"
        , Spacing.pr5
        , Spacing.pl5
        , Spacing.pt3
        ]
        [ viewPage model ]


viewPage : Model -> Html Msg
viewPage model =
    case model.page of
        IndexPage imodel ->
            Html.map IndexMsg <| Index.view imodel

        SeamCarvingPage scmodel ->
            Html.map SeamCarvingMsg <| SeamCarving.view scmodel

        TechnologiesPage tmodel ->
            Html.map TechnologiesMsg <| Technologies.view tmodel

        NotFound ->
            div [] [ text "Not Found" ]


viewNavbar : Model -> Html Msg
viewNavbar model =
    Navbar.config NavbarMsg
        |> Navbar.withAnimation
        |> Navbar.collapseMedium
        |> Navbar.dark
        |> Navbar.brand
            [ href "/" ]
            [ text " Covid Projects"
            ]
        |> Navbar.items
            [ Navbar.itemLink
                [ href <| Routes.toString Routes.SeamCarving ]
                [ text "Seam Carving" ]
            , Navbar.itemLink
                [ href <| Routes.toString Routes.Technologies ]
                [ text "Technologies" ]
            ]
        |> Navbar.customItems
            [ Navbar.customItem <|
                img
                    [ src "assets/images/github.png"
                    , class "d-inline-block align-top"
                    , style "width" "40px"
                    , style "cursor" "pointer"
                    , Spacing.mr2
                    , onClick (UrlRequest (Browser.External "https://github.com/samlee64"))
                    ]
                    []
            , Navbar.customItem <|
                img
                    [ src "assets/images/linkedin.png"
                    , class "d-inline-block align-top"
                    , style "width" "40px"
                    , style "cursor" "pointer"
                    , onClick (UrlRequest (Browser.External "https://www.linkedin.com/in/samuellee18/"))
                    ]
                    []
            ]
        |> Navbar.view model.navbarState
