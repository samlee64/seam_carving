module Page.Technologies exposing (..)

import Bootstrap.Card as Card
import Bootstrap.Card.Block as CardBlock
import Bootstrap.Utilities.Flex as Flex
import Bootstrap.Utilities.Spacing as Spacing
import Flags exposing (Flags)
import Html exposing (..)
import Html.Attributes exposing (..)


type Msg
    = NoOp


type alias Model =
    { flags : Flags }


init : Flags -> ( Model, Cmd Msg )
init flags =
    ( { flags = flags }, Cmd.none )


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        NoOp ->
            ( model, Cmd.none )


subscriptions : Model -> Sub Msg
subscriptions _ =
    Sub.none


view : Model -> Html Msg
view model =
    div []
        [ text "Built with: "
        , div [] [ text "AWS: EC2, ECR, R53" ]
        , div [] [ text "Shipped in Docker containers" ]
        , div [] [ text "Frontend: ", a [ href "https://elm-lang.org/" ] [ text "Elm" ], text " (No runtime errors, uniform code style) + JS" ]
        , div [] [ text "Backend: Node, Typescript, C++, PSQL" ]
        , div [] [ text "Icons made by ", a [ href "https://www.flaticon.com/authors/pixel-perfect" ] [ text "Pixel perfect" ], text " from ", a [ href "https://www.flaticon.com/" ] [ text "Flaticon" ] ]
        ]
