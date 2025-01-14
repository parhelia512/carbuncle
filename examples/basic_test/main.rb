include Carbuncle

Carbuncle::File.require 'input.rb'
Carbuncle::File.require 'scripts/player.rb'

class TestGame < Carbuncle::Game
  def start
    @t = 0
    load_music
    load_background
    load_text
    load_player
  end

  def load_music
    Audio.bgm_play('music/test.ogg')
  end

  def load_background
    @bmp = Bitmap.new('graphics/backgrounds/bg.jpg')
    texture = Texture.new('graphics/backgrounds/bg.jpg')
    @planes = 5.times.map do |i|
      p = Plane.new(texture)
      p.opacity = 255 - i * 20
      add_child p
      p
    end
  end

  def load_text
    Font.default_name = 'fonts/pixel-unicode.ttf'
    Font.default_size = 64
    @text = ShadowText.new
    @text.value = 'Welcöme to Carbüncle'
    @text.color.set(130, 200, 150)
    @text.position.set(
      (screen.width - @text.width) / 2,
      (screen.height - @text.height) / 2
    )
    self << @text
    @help = ShadowText.new(Font.new('fonts/pixel-unicode.ttf', 24))
    @help.value = "Move with left or right, press 'a' to attack, 's' for screenshot."
    @help.position.set(
      (screen.width - @help.width) / 2,
      (screen.height - @help.height)
    )
    add_child @help
  end

  def load_player
    @player = Game::Player.new
    @player.position.set(
      screen.width / 2,
      @text.position.y + @text.height + @player.height
    )
    add_child @player
  end

  def update(dt)
    @planes.each.with_index do |p, index|
      p.ox += (2 - index) * dt * 20
      p.oy += (3 - index) * dt * 20
    end
    super(dt)
    take_screenshot if Keyboard.press?(:s)
    @t = (@t + dt) % 2
    @text.value = @t > 1 ? 'Welcöme to Carbüncle' : 'Welcome to Carbuncle'
  end

  def take_screenshot
    Bitmap.screenshot.save('screen.png')
  end
end
